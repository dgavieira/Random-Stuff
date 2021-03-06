import sys
from hitable import Hitable, Hit_Record

class Hitable_List(Hitable):

    def __init__(self, l=None):
        """Construct a Hitable_List."""

        self.list = l

    def hit(self, r, t_min, t_max, rec):
        """Decide if any list elements have been hit.

        r      Ray of interest
        t_min  time minimum (float)
        t_max  time maximum (float)
        rec    a hit_record reference
        """

        temp_rec = Hit_Record()
        hit_anything = False
        closest_so_far = t_max

        for l in self.list:
            if l.hit(r, t_min, closest_so_far, temp_rec):
#
                print('temp_rec: %s' % str(temp_rec))
                sys.stdout.flush()
#
                hit_anything = True
                closest_so_far = temp_rec.t
                rec.update(temp_rec)

        return hit_anything

#class hitable_list: public hitable  {
#    public:
#        hitable_list() {}
#        hitable_list(hitable **l, int n) {list = l; list_size = n; }
#        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
#        hitable **list;
#        int list_size;
#};

#bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
#        hit_record temp_rec;
#        bool hit_anything = false;
#        double closest_so_far = t_max;
#        for (int i = 0; i < list_size; i++) {
#            if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
#                hit_anything = true;
#                closest_so_far = temp_rec.t;
#                rec = temp_rec;
#            }
#        }
#        return hit_anything;
#}
